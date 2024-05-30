package com.backend.api.global.security.filter;

import com.backend.api.global.common.code.ErrorCode;
import com.backend.api.global.exception.TokenErrorResponse;
import com.backend.api.global.exception.UserNotAuthorizedException;
import com.fasterxml.jackson.databind.ObjectMapper;
import io.jsonwebtoken.ExpiredJwtException;
import io.jsonwebtoken.JwtException;
import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.web.filter.OncePerRequestFilter;

import java.io.IOException;

public class ExceptionHandlerFilter extends OncePerRequestFilter {
    @Override
    protected void doFilterInternal(
            HttpServletRequest request,
            HttpServletResponse response,
            FilterChain filterChain) throws ServletException, IOException {
        try {
            filterChain.doFilter(request, response);
        } catch (ExpiredJwtException e) {
            //토큰의 유효기간 만료
            setErrorResponse(response, HttpServletResponse.SC_UNAUTHORIZED, ErrorCode.EXPIRED_JWT);
        } catch (JwtException | IllegalArgumentException e) {
            //유효하지 않은 토큰
            setErrorResponse(response, HttpServletResponse.SC_UNAUTHORIZED, ErrorCode.TOKEN_NOT_VALID);
        } catch (UserNotAuthorizedException e) {
            setErrorResponse(response, ErrorCode.USER_NOT_AUTHORIZED.getStatus(), ErrorCode.USER_NOT_AUTHORIZED);
        }
    }

    private void setErrorResponse(
            HttpServletResponse response,
            int codeNum,
            ErrorCode errorCode
    ) {
        ObjectMapper objectMapper = new ObjectMapper();
        response.setStatus(codeNum);
        response.setContentType("application/json; charset=UTF-8");
        TokenErrorResponse errorResponse = new TokenErrorResponse(errorCode);
        try {
            response.getWriter().write(objectMapper.writeValueAsString(errorResponse));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
