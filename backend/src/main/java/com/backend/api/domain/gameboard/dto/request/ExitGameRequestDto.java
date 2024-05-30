package com.backend.api.domain.gameboard.dto.request;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "게임 나가기 RequestDto")
public record ExitGameRequestDto(
        @Schema(description = "게임 ID")
        Long gameId
) {
}
