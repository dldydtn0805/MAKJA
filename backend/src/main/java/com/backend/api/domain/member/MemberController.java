package com.backend.api.domain.member;

import com.backend.api.domain.member.dto.request.LoginRequestDto;
import com.backend.api.domain.member.dto.request.ReissueRequestDto;
import com.backend.api.domain.member.dto.request.SignupRequestDto;
import com.backend.api.domain.member.dto.response.LoginResponseDto;
import com.backend.api.domain.member.dto.response.SignupResponseDto;
import com.backend.api.global.common.BaseResponse;
import com.backend.api.global.common.code.SuccessCode;
import com.backend.api.global.security.SecurityMember;
import com.backend.api.global.security.util.SecurityUtil;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletRequest;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.web.bind.annotation.*;

import java.util.Map;

@Slf4j
@RestController
@RequestMapping("/api")
@RequiredArgsConstructor
@Tag(name = "멤버 API")
public class MemberController {

    private final MemberService memberService;

    @PostMapping("/signup")
    @Operation(summary = "회원가입", description = "서비스 이용을 위해 회원가입을 진행합니다.")
    public ResponseEntity<BaseResponse<SignupResponseDto>> signup(@RequestBody SignupRequestDto dto) {
        SignupResponseDto responseDto = memberService.signup(dto);
        return BaseResponse.success(SuccessCode.CREATE_SUCCESS, responseDto);
    }

    @GetMapping("/nickname-check/{nickname}")
    @Operation(summary = "아이디 중복확인", description = "해당 사이트에 동일한 아이디가 존재하는지 확인합니다.")
    public ResponseEntity<BaseResponse<Boolean>> nicknameCheck(@PathVariable("nickname") String nickname) {
        boolean result = memberService.existsNickname(nickname);
        log.info(String.valueOf(result));
        return BaseResponse.success(SuccessCode.CHECK_SUCCESS, result);
    }

    @PostMapping("/login")
    @Operation(summary = "로그인", description = "서비스 이용을 위한 로그인을 실행합니다.")
    public ResponseEntity<BaseResponse<LoginResponseDto>> login(@RequestBody LoginRequestDto dto) {
        LoginResponseDto responseDto = memberService.login(dto);
        return BaseResponse.success(SuccessCode.LOGIN_SUCCESS, responseDto);
    }

    @GetMapping("/logout")
    @Operation(summary = "로그아웃", description = "서비스를 나가기 위한 로그아웃을 실행합니다.")
    public ResponseEntity<BaseResponse<String>> logout(HttpServletRequest request, @AuthenticationPrincipal SecurityMember securityMember) {
        String accessToken = SecurityUtil.getAccessToken(request);
        memberService.logout(securityMember, accessToken);
        return BaseResponse.success(SuccessCode.LOGOUT_SUCCESS, "로그아웃 완료");
    }

    @PostMapping("/reissue")
    @Operation(summary = "토큰 재발급", description = "액세스 토큰이 만료되어 토큰을 재발급합니다.")
    public ResponseEntity<BaseResponse<Map<String, String>>> reissue(@RequestBody ReissueRequestDto dto) {
        Map<String, String> tokenMap = memberService.reissue(dto.refreshToken(), dto.nickname());
        return BaseResponse.success(SuccessCode.SUCCESS_REISSUE, tokenMap);
    }
}
