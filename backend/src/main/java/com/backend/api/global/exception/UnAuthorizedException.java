package com.backend.api.global.exception;


import com.backend.api.global.common.code.ErrorCode;

/**
 * 401
 * 로그인이 필요한 요청에 로그인을 하지 않아 발생
 */
public class UnAuthorizedException extends DefenseRuntimeException {
    protected static final String MESSAGE_KEY = "error.UnAuthorized";

    public UnAuthorizedException(String detailMessageKey, ErrorCode errorCode, Object... params) {
        super(MESSAGE_KEY + "." + detailMessageKey, errorCode, params);
    }
}
