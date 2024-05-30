package com.backend.api.global.common.code;

import lombok.Getter;

@Getter
public enum ErrorCode {
    /**
     * ******************************* Global Error CodeList ***************************************
     * HTTP Status Code
     * 400 : Bad Request
     * 401 : Unauthorized
     * 403 : Forbidden
     * 404 : Not Found
     * 409 : Conflict
     * 500 : Internal Server Error
     * *********************************************************************************************
     */
    // 잘못된 서버 요청
    BAD_REQUEST_ERROR(400, "G001", "Bad Request Exception"),

    // @RequestBody 데이터 미 존재
    REQUEST_BODY_MISSING_ERROR(400, "G002", "Required request body is missing"),

    // 유효하지 않은 타입
    INVALID_TYPE_VALUE(400, "G003", "Invalid Type Value"),

    // Request Parameter 로 데이터가 전달되지 않을 경우
    MISSING_REQUEST_PARAMETER_ERROR(400, "G004", "Missing Servlet RequestParameter Exception"),

    // 입력/출력 값이 유효하지 않음
    IO_ERROR(400, "G005", "I/O Exception"),

    // com.google.gson JSON 파싱 실패
    JSON_PARSE_ERROR(400, "G006", "JsonParseException"),

    // com.fasterxml.jackson.core Processing Error
    JACKSON_PROCESS_ERROR(400, "G007", "com.fasterxml.jackson.core Exception"),

    // 잘못된 사용자 정보
    UNAUTHORIZED(401, "F003", "사용자 정보를 찾을 수 없습니다."),
    WRONG_PASSWORD(401, "F004", "잘못된 비밀번호 입니다."),
    WRONG_ID_OR_PASSWORD(401, "F005", "잘못된 아이디/비밀번호 입니다."),
    USER_NOT_AUTHORIZED(401, "F006", "로그인 된 사용자가 없습니다."),
    TOKEN_NOT_VALID(401, "F007", "토큰이 유효하지 않습니다."),
    MALFORMED_JWT(401, "F008", "토큰이 유효하지 않습니다."),
    EXPIRED_JWT(420, "F009", "토큰이 만료되었습니다. 다시 로그인해주세요."),
    NICKNAME_NOT_VALID(401, "F011", "적절치 않은 닉네임입니다. 영어,숫자,특수문자로 다시 구성해주세요."),
    PASSWORD_NOT_VALID(401, "F012", "적절치 않은 비밀번호 설정입니다. 영어,숫자,특수문자로만 다시 구성해주세요."),

    // 권한이 없음
    INVALID_REFRESH_TOKEN_EXCEPTION(403, "B105", "유효하지 않은 REFRESH TOKEN 입니다."),
    FORBIDDEN_ERROR(403, "G008", "권한이 없습니다."),
    NOT_MY_GAME(403, "G014", "본인의 게임이 아닙니다."),

    // 이미 유효하지 않은 게임
    GAME_ALREADY_DONE(403, "G013", "이미 종료된 게임입니다."),

    // 서버로 요청한 리소스가 존재하지 않음
    NOT_FOUND_ERROR(404, "G009", "Not Found Exception"),
    USER_NOT_FOUND(404, "G010", "존재하지 않는 사용자입니다."),


    // NULL Point Exception 발생
    NULL_POINT_ERROR(404, "G011", "Null Point Exception"),

    // @RequestBody 및 @RequestParam, @PathVariable 값이 유효하지 않음
    NOT_VALID_ERROR(404, "G012", "Handle Validation Exception"),

    // Header 가 유효하지 않은 경우
    NOT_VALID_HEADER_ERROR(404, "G013", "Header에 데이터가 존재하지 않는 경우 "),
    // 로컬 파일 업로드 실패,
    FAILED_TO_UPLOAD_LOCAL_FILE(404, "F001", "로컬 파일 업로드 실패"),
    // 로컬 파일 업로드 실패,
    FAILED_TO_UPLOAD_S3_FILE(404, "F002", "S3 파일 업로드 실패"),
    // 서버가 처리 할 방법을 모르는 경우 발생
    INTERNAL_SERVER_ERROR(500, "G999", "Internal Server Error Exception");

    /**
     * ******************************* Business Error CodeList ***************************************
     */

//    // 사용자 권한 인증 실패 (CODE: 100)
//    UNAUTHORIZED_USER_EXCEPTION(403, "B100", "권한이 없는 사용자입니다."),
//    FAILED_OAUTH2_AUTHENTICATION_EXCEPTION(403, "B101", "소셜 로그인에 실패했습니다."),
//    INVALID_ACCESS_TOKEN_EXCEPTION(403, "B102", "유효하지 않은 ACCESS TOKEN 입니다."),
//    EXPIRED_ACCESS_TOKEN_EXCEPTION(403, "B103", "만료된 ACCESS TOKEN 입니다."),
//    INCONSISTENT_ACCESS_TOKEN_EXCEPTION(403, "B104", "일치하지 않는 ACCESS TOKEN 입니다."),
//    EXPIRED_REFRESH_TOKEN_EXCEPTION(403, "B106", "만료된 REFRESH TOKEN 입니다."),
//    INCONSISTENT_REFRESH_TOKEN_EXCEPTION(403, "B107", "일치하지 않는 REFRESH TOKEN 입니다."),
//    ILLEGAL_TOKEN_EXCEPTION(403, "B107", "헤더에 토큰 정보가 존재하지 않습니다."),


    /**
     * ******************************* Error Code Constructor ***************************************
     */
    // 에러 코드의 '코드 상태'을 반환한다.
    private final int status;

    // 에러 코드의 '코드간 구분 값'을 반환한다.
    private final String divisionCode;

    // 에러 코드의 '코드 메시지'을 반환한다.
    private final String message;

    // 생성자 구성
    ErrorCode(final int status, final String divisionCode, final String message) {
        this.status = status;
        this.divisionCode = divisionCode;
        this.message = message;
    }
}
