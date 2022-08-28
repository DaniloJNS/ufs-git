select 
    departamento.nome, AVG(cursa.nota)
from
    universidade.departamento
left outer join
    universidade.disciplina ON disciplina.depto_responsavel = departamento.cod_depto
left outer join
    universidade.turma ON turma.cod_disc = disciplina.cod_disc
left outer join
     universidade.cursa ON cursa.id_turma = turma.id_turma
 group by
     departamento.cod_depto;
