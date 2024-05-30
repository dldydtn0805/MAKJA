package com.backend.api.domain.gameboard.dto.request;


import com.backend.api.global.common.type.ModeType;
import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "게임 저장 RequestDto")
public record SaveGameRequestDto(
        @Schema(description = "게임 ID")
        Long gameId,
        @Schema(description = "게임 점수")
        Long score
) {
}
