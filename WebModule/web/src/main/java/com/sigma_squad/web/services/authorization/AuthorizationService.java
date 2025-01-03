package com.sigma_squad.web.services.authorization;

import com.sigma_squad.web.services.authorization.abstractions.IAuthStrategy;
import com.sigma_squad.web.services.token.abstractions.ITokenAdapter;
import com.sigma_squad.web.services.token.abstractions.ITokenGenerator;
import com.sigma_squad.web.services.token.abstractions.ITokenRemover;
import com.sigma_squad.web.services.token.abstractions.ITokenSaver;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.net.http.HttpResponse;

@Service
public class AuthorizationService {
    private final ITokenGenerator tokenGenerator;
    private final ITokenSaver tokenSaver;
    private final ITokenAdapter tokenAdapter;
    private final ITokenRemover tokenRemover;
    private final IAuthStrategy authStrategy;

    @Autowired
    public AuthorizationService(ITokenGenerator tokenGenerator, ITokenSaver tokenSaver,
                                ITokenAdapter tokenAdapter, IAuthStrategy authStrategy, ITokenRemover tokenRemover) {
        this.tokenGenerator = tokenGenerator;
        this.tokenSaver = tokenSaver;
        this.tokenAdapter = tokenAdapter;
        this.authStrategy = authStrategy;
        this.tokenRemover = tokenRemover;
    }

    public void logout(String sessionId) {
        tokenRemover.removeTokens(sessionId);
    }

    public boolean isAuthorize(String sessionId) {
        handleToken(sessionId);
        String token = tokenAdapter.getTokenBody(sessionId).getAccessToken();
        return !(token == null || token.isEmpty());
    }

    public HttpResponse<String> authorize(String authType, String sessionId) {
        return authStrategy.authorize(authType, handleToken(sessionId), sessionId);
    }

    private String handleToken(String sessionId) {
        try {
            return tokenAdapter.getTokenBody(sessionId).getAuthToken();
        } catch (Exception e) {
            String token = tokenGenerator.generateSecureToken();
            tokenSaver.saveAuthToken(token, sessionId);
            return token;
        }
    }
}
