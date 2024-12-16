package com.sigma_squad.web.services.token.exceptions;

public class TokenBodyDoNotFoundException extends RuntimeException {
    public TokenBodyDoNotFoundException() {
        super("Token Body Do Not Found!");
    }
}
