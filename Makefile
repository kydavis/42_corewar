# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mburson <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/23 19:31:26 by mburson           #+#    #+#              #
#    Updated: 2017/05/07 16:45:13 by kdavis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COREWAR = corewar
COREWAR_DIR = ./src_corewar/
COREWAR_SRC = ./src_corewar/corewar

ASM = asm
ASM_DIR = ./src_asm/
ASM_SRC = ./src_asm/asm

all: $(COREWAR) $(ASM)

$(ASM): $(ASM_SRC)
	cp $< .

$(COREWAR): $(COREWAR_SRC)
	cp $< .

$(ASM_SRC): force
	$(MAKE) -C $(ASM_DIR)

$(COREWAR_SRC): force
	$(MAKE) -C $(COREWAR_DIR)

force:
	@true

clean:
	$(MAKE) -C $(COREWAR_DIR) fclean
	$(MAKE) -C $(ASM_DIR) fclean

fclean: clean
	rm -f $(COREWAR) $(ASM)

re: fclean all

test: $(COREWAR)
	./test/corewar_unit.py corewar resources/corewar resources/champs 15000

rand: $(COREWAR)
	./test/corewar_rand.py corewar resources/corewar resources/champs 100

champions: $(ASM)
	./test/test_asm.py asm resources/champs

.PHONY: all clean fclean re
