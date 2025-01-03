package com.sigma_squad.web.authorization;

import com.fasterxml.jackson.annotation.JsonFormat;
import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.xml.crypto.Data;
import java.time.OffsetDateTime;
import java.util.Date;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class ResponseData {
    @JsonProperty("ExpiresAt")
    private OffsetDateTime expiresAt;
    @JsonProperty("ResponseStatus")
    private int responseStatus;
    @JsonProperty("AccessToken")
    private String accessToken;
    @JsonProperty("RefreshToken")
    private String refreshToken;
}
