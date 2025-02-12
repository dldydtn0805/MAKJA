package com.backend.api.global.common.code;

import lombok.Getter;

@Getter
public enum SuccessCode {
    // 조회 성공 코드 (HTTP Response: 200 OK)
    SELECT_SUCCESS(200, "200", "SELECT SUCCESS"),
    CHECK_SUCCESS(200, "200", "CHECK SUCCESS"),
    SUCCESS_REISSUE(200, "200", "REISSUE SUCCESS"),

    // 삭제 성공 코드 (HTTP Response: 200 OK)
    DELETE_SUCCESS(200, "200", "DELETE SUCCESS"),
    LOGIN_SUCCESS(200, "200", "LOGIN SUCCESS"),
    LOGOUT_SUCCESS(200, "200", "LOGOUT SUCCESS"),
    SIGNOUT_SUCCESS(200, "200", "SIGN OUT SUCCESS"),

    // 삽입 성공 코드 (HTTP Response: 201 Created)
    INSERT_SUCCESS(201, "201", "INSERT SUCCESS"),
    CREATE_SUCCESS(201, "201", "CREATE SUCCESS"),

    // 저장 성공 코드 (HTTP Response: 201 Created)
    SAVE_SUCCESS(201, "201", "SAVE SUCCESS"),

    // 매도 성공 코드(HTTP Response: 200 OK)
    SELL_SUCCESS(200, "200", "SELL SUCCESS"),
    BUY_SUCCESS(200, "200", "BUY SUCCESS"),
    ; // End

    // 성공 코드의 '코드 상태'를 반환한다.
    private final int status;

    // 성공 코드의 '코드 값'을 반환한다.
    private final String code;

    // 성공 코드의 '코드 메시지'를 반환한다.s
    private final String message;

    // 생성자 구성
    SuccessCode(final int status, final String code, final String message) {
        this.status = status;
        this.code = code;
        this.message = message;
    }
}
