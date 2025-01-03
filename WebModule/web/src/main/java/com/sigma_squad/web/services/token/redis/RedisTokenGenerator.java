package com.sigma_squad.web.services.token.redis;

import com.sigma_squad.web.services.token.abstractions.ITokenGenerator;
import org.springframework.stereotype.Service;

import java.security.SecureRandom;
import java.util.Base64;

@Service
public class RedisTokenGenerator implements ITokenGenerator {
    private static final SecureRandom RANDOM = new SecureRandom();
    private static final int DEFAULT_TOKEN_LEN = 64;
    private final int byteLength;

    public RedisTokenGenerator(int byteLength) {
        this.byteLength = byteLength;
    }

    public RedisTokenGenerator() {
        this.byteLength = DEFAULT_TOKEN_LEN;
    }

    public String generateSecureToken() {
        if (byteLength <= 0) {
            throw new IllegalArgumentException("Byte length must be greater than 0");
        }

        byte[] randomBytes = new byte[byteLength];
        RANDOM.nextBytes(randomBytes);

        return Base64.getUrlEncoder().withoutPadding().encodeToString(randomBytes);
    }
}
