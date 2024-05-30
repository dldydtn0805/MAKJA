package com.backend.api.global.exception;

import com.backend.api.global.common.code.ErrorCode;

public class NotFoundException extends DefenseRuntimeException {
    protected static final String MESSAGE_KEY = "error.NotFound";

    public NotFoundException(String detailMessageKey, ErrorCode errorCode, Object... params) {
        super(MESSAGE_KEY + "." + detailMessageKey, errorCode, params);
    }
}