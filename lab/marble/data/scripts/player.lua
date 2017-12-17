-- player.lua

properties =
{
    speed = 4.,
    cameraEntity = nil
}

function computeDirectionFromKeys(keyboard)
    local leftVec = vec3_new(1, 0, 0)
    local forwardVec = vec3_new(0, 0, 1)

    local ret = vec3()

    if keyboard:keyIsPressed(Key.Z) then
        ret = vec3_mul(leftVec, -1)
    end
    if keyboard:keyIsPressed(Key.S) then
        ret = leftVec
    end
    if keyboard:keyIsPressed(Key.Q) then
        ret = forwardVec
    end
    if keyboard:keyIsPressed(Key.D) then
        ret = vec3_mul(forwardVec, -1)
    end

    return ret
end

function update(entity, dt)
    local keyboard = getComponentKeyboard(entity)
    local moveDirection = computeDirectionFromKeys(keyboard)

    local dtFactor = dt / 1000. * properties.speed
    local moveAmount = vec3_mul(moveDirection, dtFactor)

    local rigidBody = getComponentRigidBody(entity)
    assert (rigidBody)
    rigidBody:applyCentralImpulse(moveAmount)

    assert (properties.cameraEntity)
    local camera = getComponentCamera(properties.cameraEntity)

    assert (camera)
    CameraActions.move(camera, moveDirection, properties.speed, dt)
end

