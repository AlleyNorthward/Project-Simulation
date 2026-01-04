-- @author 巷北  
-- @time 2026-01-01 23:26:45  

-- 查询张伟当前角色
SELECT e.username, r.roleName, r.description
FROM Employee e
JOIN AppUserRole ur ON e.employeeId = ur.employeeId
JOIN AppRole r ON ur.roleId = r.roleId
WHERE e.username = 'front_zhang';

-- 查询张伟对应权限
SELECT e.username, p.permName, p.description
FROM Employee e
JOIN AppUserRole ur ON e.employeeId = ur.employeeId
JOIN AppRolePermission rp ON ur.roleId = rp.roleId
JOIN AppPermission p ON rp.permId = p.permId
WHERE e.username = 'front_zhang';
