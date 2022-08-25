SELECT 
	primeiro_nome, sobrenome
FROM 
	universidade.professor
LEFT OUTER JOIN
	universidade.leciona ON leciona.mat_professor = professor.mat_professor
LEFT OUTER JOIN
	universidade.turma ON turma.id_turma = leciona.id_turma
LEFT OUTER JOIN
	universidade.usuario ON usuario.cpf = professor.cpf
WHERE
	leciona.id_turma is null;
