package com.sigma_squad.web.services.token.abstractions;

public interface ITokenSaver {
    void saveAuthToken(String token, String sessionId);
    void saveJWTTokens(JWTDTO jwtdto, String sessionId);
}
