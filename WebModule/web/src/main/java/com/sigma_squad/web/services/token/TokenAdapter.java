package com.sigma_squad.web.services.token;

import com.sigma_squad.web.services.token.exceptions.TokenBodyDoNotFoundException;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.Map;

@Service
public class TokenAdapter {
    private final RedisTemplate<String, RedisTokenDTO> redis;
    private final HttpSession session;

    @Autowired
    public TokenAdapter(RedisTemplate<String, RedisTokenDTO> redis, HttpSession session) {
        this.redis = redis;
        this.session = session;
    }

    public RedisTokenDTO getTokenBody() throws TokenBodyDoNotFoundException {
        if(!isExistTokenBody()) throw new TokenBodyDoNotFoundException();
        return redis.opsForValue().get(session.getId());
    }

    public boolean isExistTokenBody() {
        return redis.hasKey(session.getId());
    }
}
