package com.backend.api.global.security.provider;


import com.backend.api.global.security.SecurityMember;
import com.backend.api.global.security.service.CustomMemberDetailsService;
import com.backend.api.global.security.util.SecurityUtil;
import io.jsonwebtoken.*;
import io.jsonwebtoken.io.Decoders;
import io.jsonwebtoken.security.Keys;

import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.stereotype.Component;

import java.security.Key;
import java.util.*;
import java.util.stream.Collectors;


@Component
@Slf4j
public class JwtTokenProvider {

    private final long ACCESS_TOKEN_EXPIRE_TIME = 6 * 60 * 60 * 1000L; // 6시간
    private final long REFRESH_TOKEN_EXPIRE_TIME = 7 * 24 * 60 * 60 * 1000L; // 7일

    private Key key;

    @Autowired
    private CustomMemberDetailsService customMemberDetailsService;

    public JwtTokenProvider(@Value("${jwt.secret}") String secretKey) {
        byte[] keyBytes = Decoders.BASE64.decode(secretKey);
        this.key = Keys.hmacShaKeyFor(keyBytes);
    }

    public Map<String, String> generateToken(Long id, String nickname, Authentication authentication) {
        // 로그인 정보 가져오기
        SecurityMember securityMember = new SecurityMember(
                id,
                (String) authentication.getPrincipal(),
                (String) authentication.getCredentials()
        );


        String authorities = securityMember.getAuthorities()
                .stream()
                .map(GrantedAuthority::getAuthority)
                .collect(Collectors.joining(","));

        long now = (new Date()).getTime();
        Date accessTokenExpireTime = new Date(now + ACCESS_TOKEN_EXPIRE_TIME);
        System.out.println("accessTokenExpireTime = " + accessTokenExpireTime);
        String accessToken = Jwts.builder()
                .setSubject(authentication.getName())
                .claim("auth", authorities)
                .claim("id", id)
                .claim("nickname", securityMember.getNickname())
                .setExpiration(accessTokenExpireTime)
                .signWith(key, SignatureAlgorithm.HS256)
                .compact();

        log.info("access token : " + accessToken);

        Date refreshTokenExpireTime = new Date(now + REFRESH_TOKEN_EXPIRE_TIME);
        String refreshToken = Jwts.builder()
                .setExpiration(refreshTokenExpireTime)
                .signWith(key, SignatureAlgorithm.HS256)
                .compact();

        log.info("refresh token : " + refreshToken);


        HashMap<String, String> map = new HashMap<>();
        map.put("access_token", SecurityUtil.getTokenPrefix() + " " + accessToken);
        map.put("refresh_token", SecurityUtil.getTokenPrefix() + " " + refreshToken);
        return map;
    }

    // 복호화
    public Authentication getAuthentication(String token) {
        // 토큰 복호화
        Claims claims = parseClaims(token);

        String username = claims.getSubject();
        SecurityMember securityMember = (SecurityMember) customMemberDetailsService.loadUserByUsername(username);

        List<SimpleGrantedAuthority> authorities = Arrays.stream(claims.get("auth").toString().split(","))
                .map(SimpleGrantedAuthority::new)
                .collect(Collectors.toList());

        return new UsernamePasswordAuthenticationToken(securityMember, "", authorities);
    }

    // 검증
    public boolean validateToken(String token) {
        try {
            Jwts.parserBuilder().setSigningKey(key).build().parseClaimsJws(token);
            return true;
        } catch (io.jsonwebtoken.security.SecurityException | MalformedJwtException e) {
            log.info("Invalid JWT Token", e);
            throw e;
        } catch (ExpiredJwtException e) {
            log.info("Expired JWT Token", e);
            throw e;
        } catch (UnsupportedJwtException e) {
            log.info("Unsupported JWT Token", e);
            throw e;
        } catch (IllegalArgumentException e) {
            log.info("JWT claims string is empty.", e);
        }
        return false;
    }

    // 액세스 토큰으로부터 정보 추출
    private Claims parseClaims(String accessToken) {
        try {
            return Jwts.parserBuilder().setSigningKey(key).build().parseClaimsJws(accessToken).getBody();
        } catch (ExpiredJwtException e) {
            return e.getClaims();
        }
    }

    public long getACCESS_TOKEN_EXPIRE_TIME() {
        return ACCESS_TOKEN_EXPIRE_TIME;
    }

    public long getREFRESH_TOKEN_EXPIRE_TIME() {
        return REFRESH_TOKEN_EXPIRE_TIME;
    }
}
