package com.backend.api.domain.gameboard;

import com.backend.api.domain.gameboard.dto.request.CreateGameRequestDto;
import com.backend.api.domain.gameboard.dto.request.ExitGameRequestDto;
import com.backend.api.domain.gameboard.dto.request.SaveGameRequestDto;
import com.backend.api.domain.gameboard.dto.response.*;
import com.backend.api.global.common.BaseResponse;
import com.backend.api.global.common.code.SuccessCode;
import com.backend.api.global.common.type.ModeType;
import com.backend.api.global.security.SecurityMember;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import lombok.extern.log4j.Log4j2;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.web.bind.annotation.*;

@Log4j2
@RestController
@RequestMapping("/api/game")
@RequiredArgsConstructor
@Tag(name = "게임 API")
public class GameBoardController {

    private final GameBoardService gameBoardService;

    @PostMapping
    @Operation(summary = "새로운 게임 생성", description = "게임을 시작하면서 게임을 생성합니다.")
    public ResponseEntity<BaseResponse<CreateGameResponseDto>> createGame(@RequestBody CreateGameRequestDto dto, @AuthenticationPrincipal SecurityMember member) {
        String modeType = dto.modeStr().toUpperCase();
        ModeType mode = ModeType.valueOf(modeType);
        CreateGameResponseDto responseDto = gameBoardService.createGame(mode, member.getId());
        return BaseResponse.success(SuccessCode.CREATE_SUCCESS, responseDto);
    }

    @PatchMapping("/exit")
    @Operation(summary = "게임 중 나가기", description = "진행 중인 게임을 포기합니다.")
    public ResponseEntity<BaseResponse<String>> exitGame(@RequestBody ExitGameRequestDto dto, @AuthenticationPrincipal SecurityMember member) {
        String responseDto = gameBoardService.exitGame(dto, member.getId());
        return BaseResponse.success(SuccessCode.CHECK_SUCCESS, responseDto);
    }

    @PatchMapping("/save")
    @Operation(summary = "게임기록 저장", description = "게임 완료 후 게임 기록을 저장합니다.")
    public ResponseEntity<BaseResponse<SaveGameResponseDto>> saveGame(@RequestBody SaveGameRequestDto dto, @AuthenticationPrincipal SecurityMember member) {
        SaveGameResponseDto responseDto = gameBoardService.saveGame(dto, member.getId());
        return BaseResponse.success(SuccessCode.SAVE_SUCCESS, responseDto);
    }

    @GetMapping("/rank/{mode}")
    @Operation(summary = "게임 랭킹 조회", description = "각 모드에서 점수가 높은 게임 정보를 출력합니다.")
    public ResponseEntity<BaseResponse<GameResponseDto<GameRankingResponseDto>>> getGameRanking(@PathVariable("mode") String modeStr,
                                                                                                @RequestParam int pageNumber) {
        ModeType mode = ModeType.valueOf(modeStr.toUpperCase());
        GameResponseDto<GameRankingResponseDto> responseDto = gameBoardService.getGameRanking(mode, pageNumber);
        log.info("게임랭킹 조회");
        return BaseResponse.success(SuccessCode.CHECK_SUCCESS, responseDto);
    }

    @GetMapping("/{memberId}")
    @Operation(summary = "내 게임기록 조회", description = "내가 완료한 게임 목록들이 보여집니다.")
    public ResponseEntity<BaseResponse<GameResponseDto<GameInfoDto>>> getMyGame(@PathVariable("memberId") Long memberId,
                                                                                @RequestParam int pageNumber) {
        GameResponseDto<GameInfoDto> responseDto = gameBoardService.getMyGame(memberId, pageNumber);
        log.info("내 게임 조회");
        return BaseResponse.success(SuccessCode.CHECK_SUCCESS, responseDto);


    }
}
