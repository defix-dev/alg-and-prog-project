package org.ziptegrity;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

public class Main {
    private static final int NAME_COUNT = 100;
    private static final int SPEED = 2000;
    private static final int MAX_LEN = 60;
    private static final String SESSION_ID = "h20hnqgrmea6huj65bgkde80f5";

    public static void main(String[] args) throws IOException, InterruptedException {
        HttpClient client = HttpClient.newBuilder().build();
        int i = NAME_COUNT;
        while (i-- > 0) {
            int gender = new Random().nextInt(1, 3);
            HttpRequest req = HttpRequest.newBuilder()
                    .uri(URI.create(STR."https://www.behindthename.com/random/random.php?gender=\{gender == 1 ? "m" : "f"}&number=1&sets=1&surname=&all=yes"))
                    .GET()
                    .header("Cookie", STR."PHPSESSID=\{SESSION_ID}")
                    .build();
            client.sendAsync(req, HttpResponse.BodyHandlers.ofString()).thenApply(res -> {
                String name = parseName(res.body());
                if(name.length() > MAX_LEN) {
                    System.out.println("skipped\n------------------");
                    return res;
                }
                String pName = getPalindromeName(name);
                System.out.println(name);
                System.out.println(STR."--> \{pName}");
                System.out.println("------------------");
                return res;
            }).join();
            Thread.sleep(SPEED);
        }
    }

    private static String parseName(String page) {
        int startId = page.indexOf("<a href=\"/name/");
        String cutPage = page.substring(startId);
        int endId = cutPage.indexOf("</a>");
        startId = cutPage.indexOf(">");
        return cutPage.substring(startId+1, endId);
    }

    private static String getPalindromeName(String target) {
        StringBuilder palindromeBuilder = new StringBuilder(target);
        char[] tArr = target.toCharArray();
        for(int i = target.length()-2; i > 0; i--) {
            palindromeBuilder.append(tArr[i]);
        }
        return palindromeBuilder.append(String.valueOf(tArr[0]).toLowerCase()).toString();
    }
}