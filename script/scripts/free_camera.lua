-- free_camera.lua

function computeDirectionFromKeys(keyboard, camera)
    local viewVec = camera:viewVector():normalized()
    local rightVec = camera:rightVector()

    local ret = vec3()

    if keyboard:keyIsPressed(Key.Z) then
        ret = viewVec
    end
    if keyboard:keyIsPressed(Key.S) then
        ret = vec3_mul(viewVec, -1)
    end
    if keyboard:keyIsPressed(Key.Q) then
        ret = vec3_mul(rightVec, -1)
    end
    if keyboard:keyIsPressed(Key.D) then
        ret = rightVec
    end

    return ret
end

function update(entity, dt)
    camera = getComponentCamera(entity)
    keyboard = getComponentKeyboard(entity)

    local speed = 100.
    if keyboard:keyIsPressed(Key.Shift) then
        speed = speed * 4.
    end

    local moveDirection = computeDirectionFromKeys(keyboard, camera)

    CameraActions.move(camera, moveDirection, speed, dt)
end

