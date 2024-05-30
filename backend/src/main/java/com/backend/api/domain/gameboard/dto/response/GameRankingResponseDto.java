package com.backend.api.domain.gameboard.dto.response;

import com.backend.api.global.common.type.ModeType;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@EqualsAndHashCode(callSuper = false)
@Data
@NoArgsConstructor
@AllArgsConstructor // 추가
@Schema(name = "랭킹 조회 ResponseDto")
public class GameRankingResponseDto extends GameInfoDto {
    @Schema(description = "순위")
    Long rank;
    @Schema(description = "멤버 ID")
    Long memberId;
    @Schema(description = "멤버 닉네임")
    String nickname;

    public GameRankingResponseDto(Long gameId, ModeType mode, Long score, LocalDateTime startDate, Long playtime, Long rank, Long memberId, String nickname) {
        super(gameId, mode, score, startDate, playtime);
        this.rank = rank;
        this.memberId = memberId;
        this.nickname = nickname;
    }
}
