package com.backend.api.domain.gameboard.dto.response;

import com.backend.api.global.common.type.ModeType;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(name = "게임 정보 ResponseDto")
public class GameInfoDto {
    @Schema(description = "게임 ID")
    Long gameId;

    @Schema(description = "게임 난이도")
    ModeType mode;

    @Schema(description = "게임 점수")
    Long score;

    @Schema(description = "게임 시작일자")
    LocalDateTime startDate;

    @Schema(description = "게임 시간")
    Long playtime;
}
