package com.backend.api.domain.member.dto.response;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "회원가입 ResponseDto")
public record SignupResponseDto(
        @Schema(description = "회원 ID(PK)")
        Long id,
        @Schema(description = "닉네임")
        String nickname
) {
}
