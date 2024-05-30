package com.backend.api.global.exception;


import com.backend.api.global.common.code.ErrorCode;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

@Data
public class TokenErrorResponse {
    @NotNull
    private final int status;

    @NotNull
    private final String code;

    @NotNull
    private final String message;

    public TokenErrorResponse(ErrorCode errorCode) {
        this.status = errorCode.getStatus();
        this.code = errorCode.getDivisionCode();
        this.message = errorCode.getMessage();
    }
}
