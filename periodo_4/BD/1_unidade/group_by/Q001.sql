SELECT 
    professor.mat_professor, MAX(cursa.nota), MIN(cursa.nota), AVG(cursa.nota)
FROM 
    universidade.professor
JOIN
    universidade.leciona ON leciona.mat_professor = professor.mat_professor
JOIN
    universidade.turma ON turma.id_turma = leciona.id_turma
JOIN
    universidade.disciplina ON disciplina.cod_disc = turma.cod_disc
JOIN
    universidade.cursa ON cursa.id_turma = turma.id_turma
GROUP BY
    professor.mat_professor;
