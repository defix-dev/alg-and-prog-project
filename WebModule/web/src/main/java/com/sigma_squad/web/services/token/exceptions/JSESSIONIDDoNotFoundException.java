package com.sigma_squad.web.services.token.exceptions;

public class JSESSIONIDDoNotFoundException extends RuntimeException {
    public JSESSIONIDDoNotFoundException() {
        super("JSESSIONID Do Not Found!");
    }
}
