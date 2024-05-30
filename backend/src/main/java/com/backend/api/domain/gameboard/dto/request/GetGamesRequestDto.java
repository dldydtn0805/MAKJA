package com.backend.api.domain.gameboard.dto.request;

import com.backend.api.global.common.type.ModeType;
import io.swagger.v3.oas.annotations.media.Schema;

@Schema(name = "랭킹 조회 RequestDto")
public record GetGamesRequestDto(
        @Schema(description = "게임 난이도")
        ModeType mode
) {
}
