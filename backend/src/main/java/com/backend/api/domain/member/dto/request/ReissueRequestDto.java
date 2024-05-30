package com.backend.api.domain.member.dto.request;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "토큰 재발급 RequestDto")
public record ReissueRequestDto(
        @Schema(description = "refresh token")
        String refreshToken,
        @Schema(description = "닉네임")
        String nickname
) {
}
