package com.backend.api.domain.gameboard;

import com.backend.api.domain.gameboard.dto.response.GameRankingResponseDto;
import com.backend.api.domain.gameboard.dto.response.GameInfoDto;
import com.backend.api.global.common.type.ModeType;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface GameBoardRepository extends JpaRepository<GameBoard, Long> {

    GameBoard findGameBoardById(Long id);

    @Query("select g " +
            "from GameBoard g join g.member m " +
            "where m.id =:memberId and g.status = 0")
    List<GameBoard> findInGameBoard(Long memberId);


    @Query("select new com.backend.api.domain.gameboard.dto.response.GameRankingResponseDto(" +
            " g.id , g.mode, g.score, g.startDate, g.playtime, rank() over (order by g.score desc), m.id, m.nickname" +
            ") from GameBoard g join g.member m" +
            " where g.mode = :mode " +
            "and g.status = 2")
    List<GameRankingResponseDto> findGameRanking(ModeType mode);

    @Query("select new com.backend.api.domain.gameboard.dto.response.GameInfoDto(" +
            "g.id, g.mode, g.score, g.startDate, g.playtime) " +
            "from GameBoard g join g.member m " +
            "where m.id =:memberId " +
            "and g.status = 2" +
            "order by g.startDate DESC")
    List<GameInfoDto> findMyGames(Long memberId);


    @Query("select new com.backend.api.domain.gameboard.dto.response.GameInfoDto(" +
            "g.id, g.mode, g.score, g.startDate, g.playtime) " +
            "from GameBoard g join g.member m " +
            "where m.id =:memberId " +
            "and g.status = 2 " +
            "and g.mode =:mode " +
            "order by g.score DESC " +
            "limit 1")
    List<GameInfoDto> findMyBestGame(Long memberId, ModeType mode);
}
