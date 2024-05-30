package com.backend.api.global.exception;

import com.backend.api.global.common.code.ErrorCode;

public class UserNotFoundException extends NotFoundException {
    private static final ErrorCode errorCode = ErrorCode.USER_NOT_FOUND;

    public UserNotFoundException(String detailMessageKey, Object... params) {
        super(errorCode.getMessage() + " : " + detailMessageKey, errorCode, params);
    }
}
