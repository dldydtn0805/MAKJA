package com.backend.api.domain.member.dto.request;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "회원가입 RequestDto")
public record SignupRequestDto(
        @Schema(description = "닉네임")
        String nickname,
        @Schema(description = "비밀번호")
        String password
) {
}
