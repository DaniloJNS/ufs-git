select
    departamento.nome, AVG(cargo.salario)
from 
    universidade.departamento
join
    universidade.professor ON professor.departamento = departamento.cod_depto
join
    universidade.cargo ON cargo.id_cargo = professor.cargo
group by
    departamento.cod_depto;
