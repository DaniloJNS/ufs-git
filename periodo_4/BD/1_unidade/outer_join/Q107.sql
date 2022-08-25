SELECT 
	disciplina.nome, turma.turma,
	usuario.primeiro_nome, usuario.sobrenome
FROM
	universidade.disciplina 
LEFT OUTER JOIN
	universidade.disciplina disciplina2 ON disciplina2.pre_req = disciplina.cod_disc
JOIN
	universidade.turma ON turma.cod_disc = disciplina.cod_disc
JOIN
	universidade.leciona ON leciona.id_turma = turma.id_turma
JOIN
	universidade.professor ON professor.mat_professor = leciona.mat_professor
JOIN
	universidade.usuario ON usuario.cpf = professor.cpf
WHERE
	disciplina2 IS NULL;
