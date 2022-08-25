select 
	disciplina.nome, estudante.mat_estudante
from
	universidade.disciplina
join 
	universidade.turma ON turma.cod_disc = disciplina.cod_disc
join 
	universidade.cursa ON cursa.id_turma = turma.id_turma
left outer join
	universidade.estudante ON estudante.mat_estudante = cursa.mat_estudante
