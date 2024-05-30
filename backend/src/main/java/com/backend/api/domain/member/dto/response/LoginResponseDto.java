package com.backend.api.domain.member.dto.response;


import io.swagger.v3.oas.annotations.media.Schema;

import java.util.Map;

@Schema(name = "로그인 ResponseDto")
public record LoginResponseDto(
        @Schema(description = "계정 accessToken")
        Map<String, String> user
) {
}
