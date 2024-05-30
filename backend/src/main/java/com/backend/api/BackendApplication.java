package com.backend.api;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Map;

import static java.lang.System.getenv;

@EnableJpaAuditing
@SpringBootApplication
public class BackendApplication {

    public static void main(String[] args) {
        SpringApplication.run(BackendApplication.class, args);
    }

    private static Connection getConnection() throws SQLException, ClassNotFoundException {
        Map<String, String> env = getenv();
        String dbHost = env.get("MYSQL_HOST");
        String dbUser = env.get("MYSQL_USERNAME");
        String dbPassword = env.get("MYSQL_PASSWORD");
        System.out.println("dbPassword = " + dbPassword);

        Class.forName("com.mysql.cj.jdbc.Driver");
        Connection connection = DriverManager.getConnection("jdbc:mysql://" + dbHost + "/likelion", dbUser, dbPassword);
        return connection;
    }

}
