package com.sigma_squad.web.services.token;

import com.sigma_squad.web.authorization.AuthorizationController;
import com.sigma_squad.web.services.token.exceptions.JSESSIONIDDoNotFoundException;
import com.sigma_squad.web.services.token.exceptions.TokenBodyDoNotFoundException;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;

import java.util.Arrays;
import java.util.Map;
import java.util.Optional;

@Service
public class TokenSaver {
    private final RedisTemplate<String, RedisTokenDTO> redis;
    private final TokenAdapter adapter;
    private final HttpSession session;

    @Autowired
    public TokenSaver(RedisTemplate<String, RedisTokenDTO> redis, TokenAdapter adapter,
                      HttpSession session) {
        this.redis = redis;
        this.adapter = adapter;
        this.session = session;
    }

    public void saveAuthToken(String token) {
        saveToken(token, "", "", false);
    }

    public void saveJWTTokens(String accessToken, String refreshToken) {
        saveToken("", accessToken, refreshToken, true);
    }

    private void saveToken(String authToken, String accessToken, String refreshToken, boolean authenticated) {
        if(adapter.isExistTokenBody()) return;

        RedisTokenDTO dto = new RedisTokenDTO();
        dto.setAuthToken(authToken);
        dto.setAccessToken(accessToken);
        dto.setRefreshToken(refreshToken);
        dto.setStatus(authenticated ? AuthorizationController.AuthStatus.AUTHORIZED.getStatusName()
                : AuthorizationController.AuthStatus.ANONYMOUS.getStatusName());

        redis.opsForValue().set(session.getId(), dto);
    }
}
