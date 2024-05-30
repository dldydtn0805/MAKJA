package com.backend.api.domain.gameboard.dto.request;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "게임 생성 RequestDto")
public record CreateGameRequestDto(
        @Schema(description = "게임 난이도")
        String modeStr
) {
}
