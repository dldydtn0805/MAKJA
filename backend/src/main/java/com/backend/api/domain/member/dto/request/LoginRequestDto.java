package com.backend.api.domain.member.dto.request;


import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "로그인 RequestDto")
public record LoginRequestDto(
        @Schema(description = "닉네임")
        String nickname,
        @Schema(description = "계정 비밀번호")
        String password
) {
}
