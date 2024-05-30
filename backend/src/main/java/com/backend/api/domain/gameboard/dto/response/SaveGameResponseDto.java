package com.backend.api.domain.gameboard.dto.response;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "게임 저장 ResponseDto")
public record SaveGameResponseDto(
        @Schema(description = "저장된 게임 ID")
        Long gameId,
        @Schema(description = "게임 점수")
        Long score
) {
}
