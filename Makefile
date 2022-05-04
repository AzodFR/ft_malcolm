SHARED_FOLDER = ./shared

all: docker

docker:
	docker compose up -d

clean:
	docker compose down

fclean: 
	docker compose down
	make fclean -C $(SHARED_FOLDER)

add_user:
	@docker exec -it $(shell basename $(CURDIR))_$$TARGET\_1 adduser $$NEWUSER
	@printf "Attach to container and type: \e[32mecho '$$NEWUSER ALL=(ALL) ALL' > /etc/sudoers.d/$$NEWUSER\e[0m\n"

connect:
	@docker exec -it $(shell basename $(CURDIR))_$$TARGET\_1 /bin/sh

