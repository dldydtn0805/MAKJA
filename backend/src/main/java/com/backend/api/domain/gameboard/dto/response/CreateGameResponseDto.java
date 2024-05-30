package com.backend.api.domain.gameboard.dto.response;

import com.backend.api.global.common.type.ModeType;
import io.swagger.v3.oas.annotations.media.Schema;

import java.time.LocalDateTime;

@Schema(name = "게임 생성 ResponseDto")
public record CreateGameResponseDto(
        @Schema(description = "생성된 게임 ID")
        Long gameId
) {
}
