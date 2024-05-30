FROM azul/zulu-openjdk:17-latest
ARG DEFALUT_JAR
ENV JAR_FILE=${DEFALUT_JAR:-./build/libs/*SNAPSHOT.jar}
COPY ${JAR_FILE} app.jar
ENTRYPOINT java -jar app.jar