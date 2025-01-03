package com.sigma_squad.web.services.token.abstractions;

import com.sigma_squad.web.services.token.UserAuthDTO;

public interface ITokenAdapter {
    UserAuthDTO getTokenBody(String sessionId);
}
