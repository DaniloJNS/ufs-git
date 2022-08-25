select
	prof.primeiro_nome prof_primerio_nome, prof.sobrenome prof_sobrenome,
	aluno.primeiro_nome aluno_primeiro_nome, aluno.sobrenome aluno_sobrenome,
	disciplina.nome
from 
	universidade.professor
join
	universidade.usuario prof ON prof.cpf = professor.cpf
join 
	universidade.leciona ON leciona.mat_professor = professor.mat_professor
join 
	universidade.turma ON turma.id_turma = leciona.id_turma
join
	universidade.disciplina ON disciplina.cod_disc = turma.cod_disc
join
	universidade.cursa ON cursa.id_turma = turma.id_turma
join
	universidade.estudante ON estudante.mat_estudante = cursa.mat_estudante
join
	universidade.usuario aluno on aluno.cpf = estudante.cpf;
