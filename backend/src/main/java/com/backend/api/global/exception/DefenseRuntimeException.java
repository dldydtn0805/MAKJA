package com.backend.api.global.exception;

import com.backend.api.global.common.code.ErrorCode;
import lombok.Getter;

/**
 * 최상위 커스텀 에러
 */
@Getter
public abstract class DefenseRuntimeException extends RuntimeException {
    private final String messageKey;
    private final Object[] params;
    private final ErrorCode errorCode;

    public DefenseRuntimeException(String messageKey, ErrorCode errorCode, Object... params) {
        this.messageKey = messageKey;
        this.params = params;
        this.errorCode = errorCode;
    }
}
