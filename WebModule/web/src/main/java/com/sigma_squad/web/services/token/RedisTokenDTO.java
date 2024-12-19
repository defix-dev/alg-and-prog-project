package com.sigma_squad.web.services.token;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class RedisTokenDTO {
    private String authToken;
    private String accessToken;
    private String refreshToken;
    private String status;
}
