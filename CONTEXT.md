# Munin

This context defines the user-interface vocabulary specific to Munin as a terminal UI component framework. It exists to separate stable UI concepts from test-framework details layered on top of Munin.

## Language

**Component**:
A renderable Munin UI element that owns bounds, focus behavior, event handling, and JSON introspection.
_Avoid_: Widget node, control object

**Automation ID**:
A stable, authored identifier attached to a **Component** so automation can locate it independently of label text, layout position, or tree path.
_Avoid_: Label, selector text, DOM path

**Component Tree**:
A rooted hierarchy of **Components** representing one rendered Munin UI surface for one client-facing interface.
_Avoid_: Global widget registry, process-wide UI set

**Automation Surface**:
The automation-relevant facts Munin exposes about a **Component Tree** without taking on selector, waiting, or test-runner policy.
_Avoid_: Test DSL, selector engine, automation framework

**Introspection JSON**:
The JSON representation Munin emits from `to_json()` to describe the current **Automation Surface** of a **Component Tree**.
_Avoid_: Separate test-only schema, debugger-only dump

**Semantic Role**:
A stable automation-facing description of what kind of UI responsibility a **Component** fulfills, such as button, edit field, toggle, or container.
_Avoid_: Raw class name, incidental implementation type

**Accessible Name**:
The user-facing text by which a **Component** can be identified in automation, such as the visible label on a button.
_Avoid_: Rendered cell scrape, internal child text, implementation label

**Native Event Path**:
The same Munin event-routing path used for live application input, through which automated input must also travel.
_Avoid_: Direct state mutation, test-only shortcut API

**Normal Keyboard Focus Navigation**:
The Munin **Native Event Path** behavior where forward tabulation moves focus to the next focusable component and backward tabulation moves focus to the previous focusable component.
_Avoid_: Test-only focus helper, direct focus mutation

**Focus Path**:
The chain of **Components** through a **Component Tree** that currently carries focus from an owning container down to the innermost focused target.
_Avoid_: Flat global focus flag, single-node-only focus model

**Focused Leaf**:
The innermost focused **Component** at the end of the current **Focus Path**.
_Avoid_: Whole focus path, arbitrary highlighted ancestor

## Relationships

- Every **Automation ID** belongs to exactly one **Component** within the **Component Tree** being inspected
- A **Component** may exist without an **Automation ID**, but automated interaction becomes less stable when it does
- An **Automation ID** must be unique within its containing **Component Tree**, but may reappear in a different **Component Tree**
- The **Automation Surface** describes a **Component Tree** for external tools but does not decide how those tools query or drive it
- The **Automation Surface** is serialized as **Introspection JSON**
- A **Semantic Role** belongs to a **Component** as part of the **Automation Surface**
- An **Accessible Name** belongs to a **Component** as part of the **Automation Surface** when the component has user-facing identifying text
- A **Focus Path** belongs to one **Component Tree** at a time and ends at exactly one **Focused Leaf**
- Automated interaction should enter through the **Native Event Path** so the inspected **Component Tree** behaves exactly as it would for live input
- **Normal Keyboard Focus Navigation** is handled through the **Native Event Path**, not by an automation-only shortcut
- In the MVP component set, the reported component `type` is treated as the effective **Semantic Role**

## Example dialogue

> **Dev:** "This dialog has two `OK` buttons. Which one should Hugin press?"
> **Domain expert:** "The test should target the **Automation ID** on the intended **Component**, not whichever label text happens to match."

> **Dev:** "Can two different telnet clients both have a `confirm_button`?"
> **Domain expert:** "Yes — each client owns a separate **Component Tree**, so **Automation ID** uniqueness is only enforced inside that tree."

> **Dev:** "Should Munin understand Hugin selectors?"
> **Domain expert:** "No — Munin exposes the **Automation Surface**, and Hugin interprets it."

> **Dev:** "Can Hugin trigger a button by calling a helper directly?"
> **Domain expert:** "No — Hugin must use the **Native Event Path** so the test exercises the real UI behavior."

> **Dev:** "How should automated tests move focus between controls?"
> **Domain expert:** "Send the same tab or backward-tab key input a user would; Munin handles **Normal Keyboard Focus Navigation** through the **Native Event Path**."

> **Dev:** "Should automation inspect `munin::button` as a class name?"
> **Domain expert:** "No — it should rely on the **Semantic Role** reported by the **Automation Surface**."

> **Dev:** "Should Hugin read terminal cells to find the text on a button?"
> **Domain expert:** "No — the button should expose its **Accessible Name** through **Introspection JSON**."

> **Dev:** "Should static label-like content become a generic text role?"
> **Domain expert:** "No — in Munin, **Image** is the component role for static label-like content."

> **Dev:** "When Hugin asks what has focus, is that just one component?"
> **Domain expert:** "Not exactly — Munin has a **Focus Path**, but the **Focused Leaf** is the most useful single target for many tests."

> **Dev:** "Should Munin emit a separate test schema just for Hugin?"
> **Domain expert:** "No — the existing **Introspection JSON** should grow to describe the full **Automation Surface**."

> **Dev:** "Do we need a separate `role` field right now?"
> **Domain expert:** "No — for the current Munin component set, the reported `type` already serves as the effective **Semantic Role**."

## Flagged ambiguities

- "id" was used loosely to mean either a stable automation identifier or an incidental tree position; resolved: **Automation ID** means the stable authored identifier on a **Component**
- "UI" was used loosely to mean either one client-facing interface or all connected interfaces in the process; resolved: **Component Tree** means one rooted UI surface
- "automation support" was used loosely to mean both data exposure and test-runner behavior; resolved: **Automation Surface** means Munin's exposed facts only
- "interaction" was used loosely to mean either user-like input or direct programmatic control; resolved: automated interaction must use the **Native Event Path**
- "focus movement" risked becoming a test-only helper; resolved: normal tab and backward-tab navigation are **Native Event Path** behavior
- "type" risked meaning either an implementation class or automation meaning; resolved: **Semantic Role** is the automation-facing meaning a **Component** exposes
- For the MVP, the reported component `type` and the effective **Semantic Role** are treated as the same thing unless the component set later proves otherwise
- "label" risked meaning either user-visible identifying text or a separate label component; resolved: **Accessible Name** is the identifying text exposed by a component itself, while label associations can be modeled separately later
- "text" risked becoming a catch-all role because terminal UIs render text; resolved: static label-like content remains an image role in Munin's current component vocabulary
- "focus" risked being flattened to a single node; resolved: Munin focus is a **Focus Path** whose innermost target is the **Focused Leaf**
- "to_json()" risked being treated as a mere debug dump; resolved: **Introspection JSON** is the serialized form of Munin's **Automation Surface**
