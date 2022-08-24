select 
	MAX(nota), disciplina.cod_disc
from 
	universidade.cursa
  join
	universidade.turma ON turma.id_turma = cursa.id_turma
  join
	universidade.disciplina ON disciplina.cod_disc = turma.cod_disc
  group by
	disciplina.cod_disc;
-- Maior nota por discplina
