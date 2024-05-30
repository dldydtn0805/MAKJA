package com.backend.api.global.exception;

import com.backend.api.global.common.code.ErrorCode;

public class UserNotAuthorizedException extends UnAuthorizedException {
    private static final ErrorCode errorCode = ErrorCode.USER_NOT_AUTHORIZED;

    public UserNotAuthorizedException(String detailMessageKey, Object... params) {
        super(errorCode.getMessage() + " : " + detailMessageKey, errorCode, params);
    }
}
