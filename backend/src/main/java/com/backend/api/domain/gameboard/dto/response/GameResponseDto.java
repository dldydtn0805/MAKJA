package com.backend.api.domain.gameboard.dto.response;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;

import java.util.List;

@AllArgsConstructor
@Getter
@Schema(name = "게임내역 ResponseDto")
public class GameResponseDto<T> {
    @Schema(description = "총 게임 수")
    int gameCnt;
    @Schema(description = "현재 페이지")
    int pageNumber;
    @Schema(description = "최고기록")
    List<T> bestScore;
    @Schema(description = "게임내역")
    List<T> gameInfo;
}
