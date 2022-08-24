select
	MIN(usuario.data_nascimento)
from 
	universidade.cursa
join
	universidade.estudante ON estudante.mat_estudante = cursa.mat_estudante
join
	universidade.usuario ON usuario.cpf = estudante.cpf
group by
	cursa.id_turma;

-- Menor data de nascimento por turma
