package com.backend.api.domain.member;

import com.backend.api.domain.member.dto.request.LoginRequestDto;
import com.backend.api.domain.member.dto.request.SignupRequestDto;
import com.backend.api.domain.member.dto.response.LoginResponseDto;
import com.backend.api.domain.member.dto.response.SignupResponseDto;
import com.backend.api.global.common.code.ErrorCode;
import com.backend.api.global.exception.BaseExceptionHandler;
import com.backend.api.global.security.SecurityMember;
import com.backend.api.global.security.provider.JwtTokenProvider;
import lombok.RequiredArgsConstructor;
import lombok.extern.log4j.Log4j2;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Collections;
import java.util.Map;
import java.util.concurrent.TimeUnit;


@Log4j2
@Service
@Transactional
@RequiredArgsConstructor
public class MemberService {

    private final MemberRepository memberRepository;
    private final PasswordEncoder passwordEncoder;
    private final JwtTokenProvider jwtTokenProvider;
    private final StringRedisTemplate redisTemplate;

    public SignupResponseDto signup(SignupRequestDto dto) {
        String nicknamePattern = "^[a-zA-Z0-9_]+$"; // 영어, 숫자, 밑줄만 허용
        boolean nicknameCheck = !dto.nickname().matches(nicknamePattern) || memberRepository.existsByNickname(dto.nickname());
        boolean passwordCheck = !dto.password().matches(nicknamePattern);
        if (nicknameCheck) {
            throw new BaseExceptionHandler(ErrorCode.NICKNAME_NOT_VALID);
        }

        if (passwordCheck) {
            throw new BaseExceptionHandler(ErrorCode.PASSWORD_NOT_VALID);
        }

        String encodePassword = passwordEncoder.encode(dto.password());

        Member member = memberRepository.save(Member.builder()
                .nickname(dto.nickname())
                .password(encodePassword)
                .build());

        return new SignupResponseDto(member.getId(), member.getNickname());
    }

    public LoginResponseDto login(LoginRequestDto dto) {
        Member member = memberRepository.findMemberByNickname(dto.nickname())
                .orElseThrow(() -> new BaseExceptionHandler(ErrorCode.UNAUTHORIZED));

        if (!passwordEncoder.matches(dto.password(), member.getPassword())) {
            throw new BaseExceptionHandler(ErrorCode.WRONG_ID_OR_PASSWORD);
        }

        String accessToken = redisTemplate.opsForValue().get("access_token:" + dto.nickname());
        boolean isDuplicateLogin = false;
        // 1. 중복 로그인 발생한 경우, 이전 로그인 정보 무효화
        if (accessToken != null) {
            isDuplicateLogin = true;
            redisTemplate.delete(("access_token:" + dto.nickname()));
            redisTemplate.delete("refresh_token:" + dto.nickname());
            redisTemplate.opsForValue().set("blackList:" + accessToken.substring(6), "logout", jwtTokenProvider.getACCESS_TOKEN_EXPIRE_TIME(), TimeUnit.MILLISECONDS);
        }

        Authentication authentication =
                new UsernamePasswordAuthenticationToken(dto.nickname(), dto.password(),
                        Collections.singleton(new SimpleGrantedAuthority("AUTHORITY")));


        Map<String, String> token = jwtTokenProvider.generateToken(member.getId(), member.getNickname(), authentication);

        token.put("id", Long.toString(member.getId()));
        token.put("nickname", member.getNickname());
        token.put("is_duplicate_login", Boolean.toString(isDuplicateLogin));

        System.out.println("token = " + token);
        redisTemplate.opsForValue().set("access_token:" + member.getNickname(), token.get("access_token"), jwtTokenProvider.getACCESS_TOKEN_EXPIRE_TIME(), TimeUnit.MILLISECONDS);
        redisTemplate.opsForValue().set("refresh_token:" + member.getNickname(), token.get("refresh_token"), jwtTokenProvider.getREFRESH_TOKEN_EXPIRE_TIME(), TimeUnit.MILLISECONDS);

        return new LoginResponseDto(token);
    }

    public void logout(SecurityMember securityMember, String accessToken) {
        redisTemplate.delete(("access_token:" + securityMember.getNickname()));
        redisTemplate.delete("refresh_token:" + securityMember.getNickname());
        redisTemplate.opsForValue().set("blackList:" + accessToken, "logout", jwtTokenProvider.getACCESS_TOKEN_EXPIRE_TIME(), TimeUnit.MILLISECONDS);
    }

    public Map<String, String> reissue(String refreshToken, String nickname) {
        if (!("Bearer " + refreshToken).equals(redisTemplate.opsForValue().get("refresh_token:" + nickname))) {
            System.out.println("refreshToken = " + refreshToken);
            throw new BaseExceptionHandler(ErrorCode.INVALID_REFRESH_TOKEN_EXCEPTION);
        }

        Member member = memberRepository.findMemberByNickname(nickname)
                .orElseThrow(() -> new BaseExceptionHandler(ErrorCode.USER_NOT_AUTHORIZED));
        Authentication authentication =
                new UsernamePasswordAuthenticationToken(member.getNickname(), member.getPassword(),
                        Collections.singleton(new SimpleGrantedAuthority("AUTHORITY")));

        Map<String, String> tokenMap = jwtTokenProvider.generateToken(member.getId(), member.getNickname(), authentication);

        tokenMap.put("id", Long.toString(member.getId()));
        tokenMap.put("nickname", member.getNickname());

        // redis 저장
        redisTemplate.opsForValue().set("access_token:" + member.getNickname(), tokenMap.get("access_token"), jwtTokenProvider.getACCESS_TOKEN_EXPIRE_TIME(), TimeUnit.MILLISECONDS);
        redisTemplate.opsForValue().set("refresh_token:" + member.getNickname(), tokenMap.get("refresh_token"), jwtTokenProvider.getREFRESH_TOKEN_EXPIRE_TIME(), TimeUnit.MILLISECONDS);
        return tokenMap;
    }

    public boolean existsNickname(String nickname) {
        String nicknamePattern = "^[a-zA-Z0-9_]+$"; // 영어, 숫자, 밑줄만 허용
        boolean nicknameCheck = nickname.matches(nicknamePattern);
        if (!nicknameCheck) {
            throw new BaseExceptionHandler(ErrorCode.NICKNAME_NOT_VALID);
        }
        return memberRepository.existsByNickname(nickname);
    }
}
