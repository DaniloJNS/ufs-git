select
    disciplina.nome, disciplina.pre_req, count(cursa.mat_estudante) qtd_aluno
 from
     universidade.disciplina
 join
     universidade.turma ON turma.cod_disc = disciplina.cod_disc
 join 
     universidade.cursa ON cursa.id_turma = turma.id_turma
 group by
     disciplina.cod_disc;
