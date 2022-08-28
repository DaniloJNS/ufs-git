select
    AVG(cursa.nota)
from
    universidade.disciplina
join
    universidade.disciplina disciplina2 ON disciplina2.pre_req = disciplina.cod_disc
join
    universidade.turma ON turma.cod_disc = disciplina2.cod_disc
join
    universidade.cursa ON cursa.id_turma = turma.id_turma
where
    cursa.nota is not null
group by
    disciplina.cod_disc;
