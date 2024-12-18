package mongodb

const (
	DEFAULT_STAGE = 0
)

func GeneratePermissions(roleName string) ([]string, error) {
	finder := ConstructRoleFinder()
	role, err := finder.FindRoleByName(roleName)
	if err != nil {
		return nil, err
	}
	if role.Stage == DEFAULT_STAGE {
		return role.Permissions, nil
	}
	tempStage := role.Stage - 1
	var permissions []string
	for tempStage > 0 {
		roles, err := finder.FindRolesByStage(tempStage)
		if err != nil {
			return nil, err
		}
		for _, tempRole := range *roles {
			permissions = append(permissions, tempRole.Permissions...)
		}
		tempStage--
	}
	return permissions, nil
}
