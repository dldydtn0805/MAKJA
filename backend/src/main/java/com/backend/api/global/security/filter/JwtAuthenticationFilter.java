package com.backend.api.global.security.filter;


import com.backend.api.domain.member.Member;
import com.backend.api.domain.member.MemberRepository;
import com.backend.api.global.exception.UserNotFoundException;
import com.backend.api.global.security.SecurityMember;
import com.backend.api.global.security.provider.JwtTokenProvider;
import com.backend.api.global.security.util.SecurityUtil;
import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.ServletResponse;
import jakarta.servlet.http.HttpServletRequest;
import lombok.RequiredArgsConstructor;
import lombok.extern.log4j.Log4j2;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.filter.GenericFilterBean;

import java.io.IOException;

@Log4j2
@RequiredArgsConstructor
public class JwtAuthenticationFilter extends GenericFilterBean {

    private final JwtTokenProvider jwtTokenProvider;
    private final MemberRepository memberRepository;
    private final StringRedisTemplate redisTemplate;

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain) throws
            ServletException,
            IOException {
        // 1. 액세스 토큰 추출
        String accessToken = SecurityUtil.getAccessToken((HttpServletRequest) request);

        // 2. 유효성 검사
        // 로그아웃된 토큰은 제외
        if (Boolean.FALSE.equals(redisTemplate.hasKey("blackList:" + accessToken))) {
            if (accessToken != null && jwtTokenProvider.validateToken(accessToken)) {
                if (accessToken.length() > 150) {
                    Authentication jwtAuthentication = jwtTokenProvider.getAuthentication(accessToken);
                    Member member = memberRepository.findMemberByNickname(jwtAuthentication.getName())
                            .orElseThrow(() -> new UserNotFoundException(""));

                    SecurityMember securityMember = new SecurityMember(member);
                    Authentication customAuthentication = new UsernamePasswordAuthenticationToken(
                            securityMember, "", securityMember.getAuthorities()
                    );

                    // securityContext에 전역 저장
                    SecurityContextHolder.getContext().setAuthentication(customAuthentication);
                }
            } else {

            }
        }
        filterChain.doFilter(request, response);
    }
}
