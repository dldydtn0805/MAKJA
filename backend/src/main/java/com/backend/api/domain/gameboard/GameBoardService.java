package com.backend.api.domain.gameboard;

import com.backend.api.domain.gameboard.dto.request.ExitGameRequestDto;
import com.backend.api.domain.gameboard.dto.request.SaveGameRequestDto;
import com.backend.api.domain.gameboard.dto.response.*;
import com.backend.api.domain.member.Member;
import com.backend.api.domain.member.MemberRepository;
import com.backend.api.global.common.code.ErrorCode;
import com.backend.api.global.common.type.ModeType;
import com.backend.api.global.common.type.StatusType;
import com.backend.api.global.exception.BaseExceptionHandler;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

@Service
@Transactional
@RequiredArgsConstructor
public class GameBoardService {

    private final GameBoardRepository gameBoardRepository;
    private final MemberRepository memberRepository;

    public CreateGameResponseDto createGame(ModeType mode, Long id) {
        // 1. 해당 member id의 게임 중, 진행중(INGAME) 상태의 게임이 있는지 조회
        List<GameBoard> gameboards = gameBoardRepository.findInGameBoard(id);

        // 2. 있다면, 해당 게임(혹은 게임들)을 포기(GIVEUP)상태로 만들고 없다면 PASS
        if (!gameboards.isEmpty()) {
            for (GameBoard inGame : gameboards) {
                inGame.updateGameBoard(0L, StatusType.GIVEUP);
            }
        }

        // 3. 이후 게임 생성 newGame 생성
        Member member = memberRepository.findById(id)
                .orElseThrow(() -> new BaseExceptionHandler(ErrorCode.FORBIDDEN_ERROR));

        GameBoard newGame = gameBoardRepository.save(GameBoard.builder()
                .member(member)
                .score(0L)
                .mode(mode)
                .status(StatusType.INGAME)
                .build()
        );
        return new CreateGameResponseDto(newGame.getId());
    }

    public String exitGame(ExitGameRequestDto dto, Long id) {
        GameBoard gameBoard = gameBoardRepository.findGameBoardById(dto.gameId());
        if (!Objects.equals(id, gameBoard.getMember().getId())) {
            throw new BaseExceptionHandler(ErrorCode.NOT_MY_GAME);
        }

        if (gameBoard.getStatus() != StatusType.INGAME) {
            throw new BaseExceptionHandler(ErrorCode.GAME_ALREADY_DONE);
        }

        gameBoard.updateGameBoard(0L, StatusType.GIVEUP);
        return "게임 나가기 완료";
    }


    public SaveGameResponseDto saveGame(SaveGameRequestDto dto, Long id) {
        GameBoard gameBoard = gameBoardRepository.findGameBoardById(dto.gameId());
        if (!Objects.equals(id, gameBoard.getMember().getId())) {
            throw new BaseExceptionHandler(ErrorCode.NOT_MY_GAME);
        }

        if (gameBoard.getStatus() != StatusType.INGAME) {
            throw new BaseExceptionHandler(ErrorCode.GAME_ALREADY_DONE);
        }

        gameBoard.updateGameBoard(dto.score(), StatusType.COMPLETE);
        return new SaveGameResponseDto(dto.gameId(), dto.score());
    }

    public GameResponseDto<GameRankingResponseDto> getGameRanking(ModeType mode, int pageNumber) {
        List<GameRankingResponseDto> games = gameBoardRepository.findGameRanking(mode);
        int gameCnt = games.size();
        int fromIndex = 0;
        int toIndex = gameCnt;

        if (pageNumber > 0) {
            fromIndex = (pageNumber - 1) * 10;
            toIndex = Math.min(fromIndex + 10, gameCnt);
        }

        // 없는 경우 빈 배열
        if (fromIndex > toIndex) {
            toIndex = 0;
            fromIndex = 0;
        }

        List<GameRankingResponseDto> bestScore = games.subList(0, 1);
        return new GameResponseDto<>(gameCnt, pageNumber, bestScore, games.subList(fromIndex, toIndex));
    }

    public GameResponseDto<GameInfoDto> getMyGame(Long memberId, int pageNumber) {
        List<GameInfoDto> games = gameBoardRepository.findMyGames(memberId);
        List<GameInfoDto> myGames = new ArrayList<>();
        ModeType[] modes = ModeType.values();
        for (ModeType mode : modes) {
            List<GameInfoDto> gameBoards = gameBoardRepository.findMyBestGame(memberId, mode);
            myGames.addAll(gameBoards);
        }

        int gameCnt = games.size();
        int fromIndex = 0;
        int toIndex = gameCnt;

        if (pageNumber > 0) {
            fromIndex = (pageNumber - 1) * 10;
            toIndex = Math.min(fromIndex + 10, gameCnt);
        }

        return new GameResponseDto<>(gameCnt, pageNumber, myGames, games.subList(fromIndex, toIndex));
    }
}
