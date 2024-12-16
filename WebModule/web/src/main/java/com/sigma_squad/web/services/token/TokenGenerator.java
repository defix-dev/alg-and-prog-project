package com.sigma_squad.web.services.token;

import java.security.SecureRandom;
import java.util.Base64;
import java.util.Random;

public class TokenGenerator {
    private static final SecureRandom RANDOM = new SecureRandom();
    private static final int DEFAULT_TOKE_LEN = 64;

    public static String generateSecureToken(int byteLength) {
        if (byteLength <= 0) {
            throw new IllegalArgumentException("Byte length must be greater than 0");
        }

        byte[] randomBytes = new byte[byteLength];
        RANDOM.nextBytes(randomBytes);

        return Base64.getUrlEncoder().withoutPadding().encodeToString(randomBytes);
    }

    public static String generateSecureToken() {
        return generateSecureToken(DEFAULT_TOKE_LEN);
    }
}
