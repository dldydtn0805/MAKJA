pipeline {
	agent any
	tools {
		jdk "zulu_jdk17.0.11"
		gradle "Gradle 8.7"
		dockerTool "DockerDefault"
	}

	environment {
		GIT_URL = "https://lab.ssafy.com/s10-final/S10P31A208.git"
		GIT_BRANCH = "develop"
		CREDENTIAL = "A208"

		BACK_NAME = "makja_be"
		BACK_PORT = "8080"
		BACK_DOCKER_PORT = "8080"
		DOCKER_ENV_DIR="/var/jenkins_home/workspace/env/"
		BACK_ENV_FILE="back.env"
		BACK_DIR = "./backend/"

		MATTERMOST_ENDPOINT="https://meeting.ssafy.com/hooks/gyubi8yb3tf6pm4uzez91ybx7e"
		MATTERMOST_CHANNEL="A208-Jenkins"
	}

	stages {
////// Git
		stage("Git Clone") {
			steps {
				echo "Git Clone Start"
				git branch : "${GIT_BRANCH}", credentialsId: "${CREDENTIAL}", url: "${GIT_URL}"
				echo "Git Clone End"

				sh '''
					echo Print env
					echo env of backend
					cat ${DOCKER_ENV_DIR}${BACK_ENV_FILE}
				'''

			}
		}




////// BE
		stage("BE : Build") {
			steps {
				echo "BE : Build Start"

				dir("${BACK_DIR}") {
					sh '''
						chmod +x gradlew
						./gradlew clean build
					'''
				}

				echo "BE : Build End"
			}
		}

		stage("BE : rm") {
			steps {
				echo "BE : rm Start"

				echo "Container"
				script {
					def running = sh(script: "docker ps -aqf name=${BACK_NAME}", returnStdout: true).trim()
					sh "echo ${running}"

					if(running) {
						sh '''
							echo docker stop ${BACK_NAME}
							docker stop ${BACK_NAME}

							echo docker rm ${BACK_NAME}
							docker rm ${BACK_NAME}
						'''
					}else {
						sh "echo no running"
					}
				}

				echo "Image"
				script {
					def image = sh(script: "docker images -aqf reference=${BACK_NAME}", returnStdout: true).trim()
					sh "echo ${image}"

					if(image) {
						sh "docker rmi ${image}"
					}else {
						sh "echo no image"
					}
				}

				echo "BE : rm End"
			}
		}

		stage("BE : Docker") {
			steps {
				echo "BE : Docker Build Start"
				dir("${BACK_DIR}") {
					script {
						sh "docker build -t ${BACK_NAME}:latest ./"
					}
				}
				echo "BE : Docker Build End"
			}
		}

		stage("BE : Container") {
			steps {
				sh "docker run --name ${BACK_NAME} --env-file ${DOCKER_ENV_DIR}${BACK_ENV_FILE} --detach --publish ${BACK_PORT}:${BACK_DOCKER_PORT} ${BACK_NAME}"
			}
		}




	}
////// Message
	post {
		success {
			script {
				def Author_ID = sh(script: "git show -s --pretty=%an", returnStdout: true).trim()
				def Author_Name = sh(script: "git show -s --pretty=%ae", returnStdout: true).trim()
				mattermostSend (
					color: "good",
					message: "빌드 성공: ${env.JOB_NAME} #${env.BUILD_NUMBER} by ${Author_ID}(${Author_Name})\n(<${env.BUILD_URL}|Details>)",
					endpoint: "${MATTERMOST_ENDPOINT}",
					channel: "${MATTERMOST_CHANNEL}"
				)
			}
		}
		failure {
			script {
				def Author_ID = sh(script: "git show -s --pretty=%an", returnStdout: true).trim()
				def Author_Name = sh(script: "git show -s --pretty=%ae", returnStdout: true).trim()
				mattermostSend (
					color: "danger",
					message: "빌드 실패: ${env.JOB_NAME} #${env.BUILD_NUMBER} by ${Author_ID}(${Author_Name})\n(<${env.BUILD_URL}|Details>)",
					endpoint: "${MATTERMOST_ENDPOINT}",
					channel: "${MATTERMOST_CHANNEL}"
				)
			}
		}
	}
}
